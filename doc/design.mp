outputtemplate := "%j-%c.mps";
input metauml;
beginfig(1);
  Class.MarkerSet("MarkerSet")(
      "-name: string",
      "-markers: Vector<AbstractMarker>",
      "-links: Vector<Link>",
      "-rootSegment: shared_ptr<Segment>",
      "-pose: Pose"
      )();

  EClass.AbstractMarker(iAbstractClass)("AbstractMarker")(
      "-id: int",
      "-name: string",
      "-color: color",
      "-physicalColor: color",
      "-size: double",
      "-optional: bool"
      )();
  Class.Marker("Marker")()();
  EClass.AbstractVirtualMarker(iAbstractClass)("AbstractVirtualMarker")(
      "-originMarkerId: int",
      "-longAxisMarker: int",
      "-planeAxisMarker: int"
      )();
  Class.VirtualMarkerRatio("VirtualMarkerRatio")(
      "-weight: Vector")();
  Class.VirtualMarkerMeasured("VirtualMarkerMeasured")(
      "-offset: Vector")();
  Class.VirtualMarkerRelativeToBone("VirtualMarkerRelativeToBone")(
      "-segment: id")();

  Class.Link("Link")(
      "-name: string",
      "-color: color",
      "-type: int",
      "-Marker1: int",
      "-Marker2: int",
      "-minLength: double",
      "-maxLength: double",
      "-extraStretch: double"
      )();

  Class.Segment("Segment")(
      "-id: int",
      "-name: string",
      "-children: shared_ptr<Segment>",
      "-originMarker: weak_ptr<Marker>",
      "-longAxisMarker: weak_ptr<Marker>",
      "-planeAxisMarker: weak_ptr<Marker>",
      "-rotationOffset: Vector3d"
      )();

  Class.Pose("Pose")("-positions: map<int, Eigen::Vector>")();

  MarkerSet.n = (0, 0);

  AbstractMarker.w = MarkerSet.e + (50, 0);
  Marker.n = AbstractMarker.sw + (0, -50);
  AbstractVirtualMarker.n = AbstractMarker.se + (0, -50);

  VirtualMarkerMeasured.n = AbstractVirtualMarker.s + (0, -50);

  VirtualMarkerRatio.e = VirtualMarkerMeasured.w - (10, 0);
  VirtualMarkerRelativeToBone.w = VirtualMarkerMeasured.e + (10, 0);

  Link.w = AbstractMarker.e + (50, 0);
  Segment.w = Link.e + (50, 0);

  Pose.w = Segment.e + (50, 0);

  drawObjects(
      MarkerSet,

      AbstractMarker,
      Marker,
      AbstractVirtualMarker,
      VirtualMarkerRatio,
      VirtualMarkerMeasured,
      VirtualMarkerRelativeToBone,

      Link,

      Segment,

      Pose);

  link(inheritance)(Marker.n -- AbstractMarker.s);
  link(inheritance)(AbstractVirtualMarker.n -- AbstractMarker.s);

  link(inheritance)(VirtualMarkerRatio.n -- AbstractVirtualMarker.s);
  link(inheritance)(VirtualMarkerMeasured.n -- AbstractVirtualMarker.s);
  link(inheritance)(VirtualMarkerRelativeToBone.n -- AbstractVirtualMarker.s);

  link(aggregation)(MarkerSet.e -- AbstractMarker.w);

  link(association)(MarkerSet.n -- MarkerSet.n - (0, -50));
  link(association)(MarkerSet.n - (0, -50) -- MarkerSet.n - (-725, -50));

  link(aggregation)(Pose.n - (0, -70) -- Pose.n);
  link(aggregation)(Link.n - (0, -34) -- Link.n);
  link(aggregation)(Segment.n - (0, -39) -- Segment.n);
  link(aggregation)(AbstractMarker.n - (0, -47) -- AbstractMarker.n);
endfig;
end
